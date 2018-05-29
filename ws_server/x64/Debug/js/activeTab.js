$(function () {
    var tabContainers = $('div.tabs > div'); // получаем массив контейнеров
    tabContainers.hide().filter(':first').show(); // прячем все, кроме первого
    // далее обрабатывается клик по вкладке
    $('div.tabs ul.tab-autho a').click(function () {
        tabContainers.hide(); // прячем все табы
        tabContainers.filter(this.hash).show(); // показываем содержимое текущего
        $('div.tabs ul.tab-autho a').removeClass('selected'); // у всех убираем класс 'selected'
        $(this).addClass('selected'); // текушей вкладке добавляем класс 'selected'
        return false;
    }).filter(':first').click();
});